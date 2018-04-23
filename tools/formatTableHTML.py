#!/usr/bin/env python3

import re

c = ""

with open("docs/table.html", mode="r") as f:
    c += f.read()

c = c.replace("discussion-timeline js-quote-selection-container","").replace("container new-discussion-timeline experiment-repo-nav", "")

with open("docs/table.html", mode="w") as f:
    f.write(c)

