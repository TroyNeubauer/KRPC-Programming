#!/usr/bin/env python3

import argparse
import sys
import os
import subprocess
import platform

parser = argparse.ArgumentParser(description='This is a script for generating project files')
parser.add_argument('--compiler', help='Determines which compiler is used when generating buildscripts [clang, gcc, msc]')
parser.add_argument('--target', help='Specifies to premake which scripts to generate [gmake2, vs2019, xcode, etc]')
parser.add_argument('--premakepath', help='The path to a premake5 executable')
parser.add_argument('--server', help='Runs a HTTP server to allow for running the build from emscripten', action='store_true')
parser.add_argument('--clean', help='Deletes all project files and binaries', action='store_true')


results = parser.parse_args()

if results.server:
	print("Starting server")
	if platform.system() == "Windows":
		os.system("python -m http.server")
	else:
		os.system("python3 -m http.server")

else:
	for line in os.popen("git submodule").read().splitlines():
		line = line.strip()
		if "Hazel" in line:
			subParts = line.split(" ")
			if (len(subParts) < 2):
				print("Error: bad line from git submodule: " + line)
				os.exit(-1)
			else:
				hazelPath = subParts[1] + "/"

	if hazelPath == None:
		print("Invalid Hazel project! Hazel is not a git submodule: ")
		os.exit(-1)

	if platform.system() == "Windows":
		osName = "windows"
	elif platform.system() == "Linux":
		osName = "linux"
	elif platform.system() == "Darwin":
		osName = "osx"
	else:
		print ("Error: Unknown system: " + platform.system())
		os.exit(-1)

	if results.target == None:
		if platform.system() == "Windows":
			target = "vs2019"
		elif platform.system() == "Linux":
			target = "gmake2"
		elif platform.system() == "Darwin":
			target = "xcode"
	else:
		target = results.target

	if results.compiler == None:
		if platform.system() == "Windows":
			compiler = "msc"
		elif platform.system() == "Linux":
			compiler = "gcc"
		elif platform.system() == "Darwin":
			compiler = "clang"
	else:
		compiler = results.compiler



	if results.premakepath == None:
		if platform.system() == "Windows":
			premakepath = hazelPath + "vendor\\premake\\bin\\premake5.exe"
		else:
			premakepath = "premake5"
	else:
		premakepath = results.premakepath

	if platform.system() == "Windows":
		premakepath = "call " + premakepath

	if compiler == "emcc":
		command = premakepath + " --os=emscripten gmake2"
	elif target == "android":
		command = premakepath + " android-studio"
	else:
		command = premakepath + " --os=" + osName + " --compiler=" + compiler + " " + target

	if results.clean:
		command = premakepath + " clean"

	if platform.system() == "Windows":
		command = command.replace("/", "\\")


	print("Running cmd: " + command)
	os.system(command)

