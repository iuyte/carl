#!/usr/bin/env python3

import sys, cleanuphelp

args = sys.argv[1:]
assert len(args) > 1, "Please speficy the input log and output html files"

f1 = args[0]
f2 = args[1]

print("Cleaning up:\n" + f1 + "\n Into HTML:\n" + f2)

r=open(f1, mode="r")
outHTML = cleanuphelp.parseContent(r.read())
r.close()

out=open(f2, mode="w")
out.write(outHTML)
out.close()
