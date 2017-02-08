from libnmap.process import NmapProcess
from libnmap.parser import NmapParser, NmapParserException

targets = "192.168.0.0/24"
options = "-sP"

report = None
nmproc = NmapProcess(targets, options)
rc = nmproc.run()
if rc != 0:
	print("nmap scan failed: {0}".format(nmproc.stderr))

try:
	report = NmapParser.parse(nmproc.stdout)
except NmapParserException as e:
	print("Exception raised while parsing scan: {0}".format(e.msg))

for host in report.hosts:
	if host.is_up() and 'B8:27:EB' in host.mac:
		print host.address
		print host.mac

for host in report.hosts:
	if host.is_up():
		print host.address
		print host.mac
