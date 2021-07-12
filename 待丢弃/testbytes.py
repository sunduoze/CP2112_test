hexstr = '10ab'
data = bytes.fromhex(hexstr)

print(len(data))

for b in data:
  print(hex(b))
