begin = 2108364
end = 2108414

cmd_qdel = "qdel -j "
print 
for i in range(begin, end+1):
	if i == end:
		cmd_qdel = cmd_qdel + "%d" % i
	else: 
		cmd_qdel = cmd_qdel + "%d," % i

print cmd_qdel