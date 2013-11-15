#!/usr/bin/python

import sys
from tempfile import mkstemp
from os import remove
from shutil import copy
import re

# match all non-alphabetical or space characters
regex = re.compile('[^A-Z^a-z^\s]+')
# match all whitespace
whitespace = re.compile('\s+')

def preprocess(filename):
    # open the input file for reading
    infile = open(filename, 'r')

    # create a temporary file and open it for writing
    fh, abs_path = mkstemp()
    out = open(abs_path, 'w')

    out.write(whitespace.sub(' ', regex.sub('', infile.read())).strip().lower())

#     go through each line
#    for line in infile:
#	# strip all non-alphabetical non-space characters
#	line = regex.sub('', line)
#	# convert multiple whitespace to a single space
#	line = whitespace.sub(' ', line)
#	# write it to the temporary file
#	out.write(line.lower())
    
    # close both files
    infile.close()
    out.close()

    # remove the original input file
    remove(filename)

    # copy the temporary file in place of the input file
    copy(abs_path, filename)

def driver():
    # if we have too few arguments, show an error message
    if len(sys.argv) < 2:
        print_usage_msg()
        return

    # call our method on the filename argument
    preprocess(sys.argv[1])

def print_usage_msg():
    print 'Usage:' + sys.argv[0] + ' [input filename]'

driver()
