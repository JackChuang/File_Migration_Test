#!/usr/bin/python3

import sys, subprocess, os
from subprocess import PIPE, STDOUT

###############################################################################
# Config
###############################################################################

# Test configuration
functionCheck = False
checkFunction = None
sanityCheck = False
linearCheck = False
randomCheck = False
numRandomchecks = 0
thoroughCheck = False
x86Only = False
bothOnly = False
instrumented = None
outfile = None
verbose = False

# Environment variable strings
aarch64_func = "AARCH64_MIGRATE"
aarch64_start = "AARCH64_MIGRATE_START"
aarch64_end = "AARCH64_MIGRATE_END"
x86_64_func = "X86_64_MIGRATE"
x86_64_start = "X86_64_MIGRATE_START"
x86_64_end = "X86_64_MIGRATE_END"

###############################################################################
# Utility functions
###############################################################################

def printHelp():
	print("test-migration.py: test migrating at different sites in a binary\n")

	print("Test migrating at different instrumented functions in the binary. " \
				"The script can do two types of runs:\n"
				"  1. Do an x86-64 -> aarch64 migration\n"
				"  2. Do an x86-64 -> aarch64 migration and an " \
							"aarch64 -> x86-64 back-migration\n"
				"By default, the script will attempt both types of runs (this can " \
				"be configured using '-b' & '-x').\n")

	print("Note that this script expects that the surrounding environment has " \
				"been set up correctly, i.e., all versions of the heterogeneous " \
				"binary are in the correct file system locations.\n")

	print("Usage: ./test-migration.py <binary> [ OPTIONS ] -- <binary arguments>")
	print("Options:")
	print("  -h / --help : print help & exit")
	print("  -i file     : a file listing which functions have been instrumented*")
	print("  -f function : specify a function at which to migrate")
	print("  -l          : linear check -- test migration at the same function for both architectures, for all available functions")
	print("  -r num      : random check -- test migration at random functions for each architecture (do 'num' random checks)")
	print("  -t          : thorough check -- test migration at every combination of functions for both binaries (WARNING: will grow O(n^2) w/ number of functions!)")
	print("  -s          : sanity check -- do homogeneous run with no migrations")
	print("  -b          : only do x86-64 -> aarch64 / aarch64 -> x86-64 run (only applies to -f & -l)")
	print("  -x          : only do x86-64 -> aarch64 migration (only applies to -f & -l)")
	print("  -o file     : store testing output to file (default is /dev/null)")
	print("  -v          : print verbose output\n")

	print("* If no file is supplied, the script will use 'readelf' to get " \
				"functions in the binary.  This is inaccurate, as it will try to " \
				"migrate at all functions, including library functions which are " \
				"known to not be instrumented, which will result in a homogeneous " \
				"run.  This function listing can be generated by the stack depth " \
				"library.\n")

def getMigrationSites(binary, instrumented):
	global verbose

	# Get readelf output
	out = subprocess.check_output(["readelf", "-sW", binary])
	outlines = out.decode("utf-8").split("\n")

	# Find all function symbols
	symbols = {}
	for line in outlines:
		if "Symbol table" in line or "Num:" in line:
			continue
		else:
			toks = line.strip().split()
			if len(toks) >= 8 and toks[3] == "FUNC":
				startAddr = int(toks[1], base=16)
				if "x" in toks[2]:
					endAddr = int(toks[2], base=16)
				else:
					endAddr = int(toks[2])
				endAddr += startAddr
				symbols[toks[7]] = (startAddr, endAddr)

	# Resolve migration sites addresses
	if instrumented != None:
		instrFile = open(instrumented, 'r')
		sites = {}
		for line in instrFile:
			function = line.strip()
			sites[function] = symbols[function]
		instrFile.close()
	else:
		sites = symbols

	print(binary + ": found " + str(len(sites)) + " migration site(s)")
	if verbose:
		for site in sorted(sites):
			print("  " + site + ": " + hex(sites[site][0]) + "-" + hex(sites[site][1]))
	return sites

def runBinary(binary, binaryArgs, outfile, siteEnv):
	global aarch64_func, aarch64_start, aarch64_end
	global x86_64_func, x86_64_start, x86_64_end

	# Record test configuration
	output = binary + ", args=" + str(binaryArgs)
	if aarch64_start in siteEnv:
		output += ", aarch64=" + siteEnv[aarch64_func] + " (" + siteEnv[aarch64_start] + "-" + siteEnv[aarch64_end] + ")"
	if x86_64_start in siteEnv:
		output += ", x86_64=" + siteEnv[x86_64_func] + " (" + siteEnv[x86_64_start] + "-" + siteEnv[x86_64_end] + ")"
	output += ":\n\n--- begin execution ---\n"

	if outfile:
		fp = open(outfile, 'a')
		fp.write(output + "\n")
	else:
		print(output)

	# Run & record test output
	cmd = os.path.abspath(binary)
	for arg in binaryArgs:
		cmd += " " + arg
	p = subprocess.Popen(cmd, \
											 shell=True, \
											 env=siteEnv, \
											 stdout=PIPE, \
											 stderr=STDOUT)
	stdout = p.communicate()[0]
	if outfile:
		fp.write(stdout.decode("utf-8", "replace"))
		fp.write("\n---- end execution ----\n\n")
		fp.close()
	else:
		print(stdout.decode("utf-8", "replace"))
		print("---- end execution ----\n")

# Note: doesn't make sense to only migrate from aarch64 -> x86-64, application
# must start on x86-64 and must migrate to aarch64 before migrating back

def runWithX86Migrating(binary, binaryArgs, outfile, func, site):
	global x86_64_func, x86_64_start, x86_64_end

	siteEnv = os.environ.copy()
	siteEnv[x86_64_func] = func
	siteEnv[x86_64_start] = hex(site[0])
	siteEnv[x86_64_end] = hex(site[1])

	runBinary(binary, binaryArgs, outfile, siteEnv)

def runWithBothMigrating(binary, binaryArgs, outfile, funcA, siteA, funcB, siteB):
	global aarch64_func, aarch64_start, aarch64_end
	global x86_64_func, x86_64_start, x86_64_end

	siteEnv = os.environ.copy()
	siteEnv[aarch64_func] = funcA
	siteEnv[aarch64_start] = hex(siteA[0])
	siteEnv[aarch64_end] = hex(siteA[1])
	siteEnv[x86_64_func] = funcB
	siteEnv[x86_64_start] = hex(siteB[0])
	siteEnv[x86_64_end] = hex(siteB[1])

	runBinary(binary, binaryArgs, outfile, siteEnv)

###############################################################################
# Test drivers
###############################################################################

def doFunctionCheck(binary, binaryArgs, outfile, function, sites, bothOnly, x86Only):
	if not bothOnly:
		runWithX86Migrating(binary, binaryArgs, outfile, function, sites[function])
	if not x86Only:
		runWithBothMigrating(binary, binaryArgs, outfile, function, sites[function], function, sites[function])

def doLinearCheck(binary, binaryArgs, outfile, sites, bothOnly, x86Only):
	for function in sorted(sites):
		if not bothOnly:
			runWithX86Migrating(binary, binaryArgs, outfile, function, sites[function])
		if not x86Only:
			runWithBothMigrating(binary, binaryArgs, outfile, function, sites[function], function, sites[function])

def doRandomCheck(binary, binaryArgs, outfile, sites, numChecks):
	print("Not yet implemented")
	assert False

def doThoroughCheck(binary, binaryArgs, outfile, sites):
	print("Not yet implemented")
	assert False

###############################################################################
# Driver
###############################################################################

if len(sys.argv) < 3:
	if len(sys.argv) == 2 and (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
		retcode = 0
	else:
		print("ERROR: please supply a binary!")
		retcode = 1
	printHelp()
	sys.exit(retcode)

binary = sys.argv[1]
binaryArgs = []

if not os.path.isfile(binary):
	print("ERROR: no binary file '" + binary + "'")
	printHelp()
	sys.exit(1)

skip = True
isBinaryArg = False
for i in range(len(sys.argv)):
	if skip:
		skip = False
		continue
	elif isBinaryArg:
		binaryArgs.append(sys.argv[i])
		continue
	elif sys.argv[i] == "-h" or sys.argv[i] == "--help":
		printHelp()
		sys.exit(0)
	elif sys.argv[i] == "-i":
		instrumented = sys.argv[i+1]
		skip = True
		continue
	elif sys.argv[i] == "-f":
		functionCheck = True
		checkFunction = sys.argv[i+1]
		skip = True
		continue
	elif sys.argv[i] == "-l":
		linearCheck = True
		continue
	elif sys.argv[i] == "-r":
		randomCheck = True
		numRandomChecks = int(sys.argv[i+1])
		skip = True
		continue
	elif sys.argv[i] == "-t":
		thoroughCheck = True
		continue
	elif sys.argv[i] == "-s":
		sanityCheck = True
		continue
	elif sys.argv[i] == "-b":
		bothOnly = True
		continue
	elif sys.argv[i] == "-x":
		x86Only = True
		continue
	elif sys.argv[i] == "-o":
		outfile = sys.argv[i+1]
		skip = True
		continue
	elif sys.argv[i] == "-v":
		verbose = True
		continue
	elif sys.argv[i] == "--":
		isBinaryArg = True
		continue

migrationSites = getMigrationSites(binary, instrumented)

if sanityCheck:
	runBinary(binary, binaryArgs, outfile, os.environ)
if functionCheck:
	doFunctionCheck(binary, \
									binaryArgs, \
									outfile, \
									checkFunction, \
									migrationSites, \
									bothOnly,
									x86Only)
if linearCheck:
	doLinearCheck(binary, \
								binaryArgs, \
								outfile, \
								migrationSites, \
								bothOnly,
								x86Only)
if randomCheck:
	doRandomCheck(binary, \
								binaryArgs, \
								outfile, \
								migrationSites, \
								numRandomChecks)
if thoroughCheck:
	doThoroughCheck(binary, \
									binaryArgs, \
									outfile, \
									migrationSites)

