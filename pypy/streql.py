# PyPy-compatible pure-python implementation

def equals(x, y):
  """Does x == y? Runtime does not depend on the bytes in the strings."""
  if isinstance(x, unicode):
    x = x.encode('utf8')
  if isinstance(y, unicode):
    y = y.encode('utf8')

  if len(x) > len(y):
    minlen = len(y)
  else:
    minlen = len(x)

  result = i = 0
  for i in xrange(minlen):
    result |= ord(x[i]) ^ ord(y[i])
  return result == 0 and len(x) == len(y)
