import os
import re

# Regex to find _Z("...") strings, handling escaped quotes
# Note: we use a non-greedy match for the content or a more robust escaped-aware match
z_pattern = re.compile(r'_Z\("((?:[^"\\]|\\.)*)"\)')

root_dir = r'C:\msys64\home\Lenovo\src\ppp\wxPSeInt'

results = []
unique_strings = set()

for filename in os.listdir(root_dir):
    file_path = os.path.join(root_dir, filename)
    if os.path.isfile(file_path) and (filename.endswith('.cpp') or filename.endswith('.h') or filename.endswith('.cxx')):
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            for i, line in enumerate(f, 1):
                matches = z_pattern.findall(line)
                for match in matches:
                    # Unescape the string if needed? User asked for "original string".
                    # Usually "original string" in lang files means the one between quotes.
                    results.append({
                        'string': match,
                        'file': filename,
                        'context': line.strip()
                    })
                    unique_strings.add(match)

# Sort unique strings
sorted_unique = sorted(list(unique_strings))

# 1. Markdown table
print("### 1. Markdown Table")
print("| String | File | Context |")
print("|--------|------|---------|")
for res in results:
    # Escape pipe in markdown table
    clean_string = res['string'].replace('|', '\|')
    clean_context = res['context'].replace('|', '\|')
    print(f"| {clean_string} | {res['file']} | {clean_context} |")

# 2. Unique list
print("\n### 2. Unique List")
for s in sorted_unique:
    print(f"- {s}")

# 3. lang/en.txt content
print("\n### 3. lang/en.txt content")
for s in sorted_unique:
    print(s)
