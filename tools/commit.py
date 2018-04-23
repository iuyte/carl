import jsonpickle
class Commit(object):
    date = ""
    commitkey = ""
    author = ""
    description = ""
    filesModified = []
    filesAdded = []
    filesDeleted = []
    def __init__(self, date, commitkey, author, description, filesModified, filesAdded, filesDeleted):
        self.date = date
        self.commitkey = commitkey
        self.author = author
        self.description = description
        self.filesModified = filesModified
        self.filesAdded = filesAdded
        self.filesDeleted = filesDeleted

def printCommit(commit):
    frozen = jsonpickle.encode(commit)
    print(frozen)

class JSONStore(object):
    def __init__(self, commit):
        self.a_timestamp = commit.date
        self.b_author = "Ethan"
        self.c_description = commit.description
        self.d_changes = []
        for file in commit.filesAdded:
            self.d_changes.append("Added: " + file)
        for file in commit.filesDeleted:
            self.d_changes.append("Deleted: " + file)
        for file in commit.filesModified:
            self.d_changes.append("Modified: " + file)

def saveList(listOfCommits):
    f = open("docs/log.json", mode="w")
    for i in range(len(listOfCommits)):
        listOfCommits[i] = JSONStore(listOfCommits[i])
    toWrite = jsonpickle.encode(listOfCommits, unpicklable=False)
    f.write(toWrite)
    f.close()
