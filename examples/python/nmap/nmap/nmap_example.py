from libnmap.process import NmapProcess


nmap_proc = NmapProcess(targets="192.168.1.0/24", options="-sP")
print nmap_proc.get_command_line()
nmap_proc.run()
print nmap_proc.summary
print nmap_proc.targets
