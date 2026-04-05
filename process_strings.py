import re

# Regex for finding the matches in the grep output
file_re = re.compile(r'^File: (.*)$')
line_re = re.compile(r'^L(\d+): (.*)$')

# Regex for extracting _Z("...") strings
# It handles escaped quotes like \"
z_re = re.compile(r'_Z\("((?:[^"\\]|\\.)*)"\)')

grep_output = """
[PASTE THE PREVIOUS GREP OUTPUT HERE - but I will read it from a file instead to avoid context bloating if it was huge, 
but here I have it in history. I will just process it in memory if possible or write it to a file.]
"""

# Actually, I'll just write a script that takes the output from a file.
