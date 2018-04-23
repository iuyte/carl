#!/usr/bin/env python3

import re

c = ""

with open("docs/table.md", mode="r") as f:
    c += f.read()

def keepChar(matchObj):
    return ", " + matchObj.group(0)[-1]

c = re.sub(r",([^\s])", keepChar, c)
c = c.replace("a_timestamp", "timestamp")
c = c.replace("b_author", "author")
c = c.replace("c_description", "description")
c = c.replace("d_changes", "changes")

with open("docs/table.md", mode="w") as f:
    f.write(c)

