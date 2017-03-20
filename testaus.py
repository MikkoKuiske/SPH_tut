import os

print [name for name in os.listdir("data") if os.path.isdir(name)]

print [x[0] for x in os.walk("./data")]
