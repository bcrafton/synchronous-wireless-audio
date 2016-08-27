import ntplib
from time import ctime
c = ntplib.NTPClient()

response = c.request('europe.pool.ntp.org', version=3)
print(response.offset)
