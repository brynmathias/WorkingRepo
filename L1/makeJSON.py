from JSON import JSON

outfile=open("goodRuns_cpp.txt",'w')

print "Parsing JSON.py"
counts = 0

#sort the JSON into event_->run order
#----------------------------

for p in JSON:
 if counts == 0: 
  outfile.write("if(   ( event_->run==%d && (\n"%int(p))
 else:
  outfile.write("   || ( event_->run==%d && (\n"%int(p))
 if len(JSON[p]) ==1:
   l = JSON[p][0]
   outfile.write( " 	(event_->lumi > %d && event_->lumi < %d)\n"%(l[0],l[1]))
 else:
   l = JSON[p][0]
   outfile.write( " 	(event_->lumi > %d && event_->lumi < %d)\n"%(l[0],l[1]))
   for l in (JSON[p])[1:]:
	 outfile.write( "  	|| (event_->lumi > %d && event_->lumi < %d)\n" %(l[0],l[1]))
 outfile.write( "   	)\n")
 outfile.write( "  	)\n")
 counts+=1

outfile.write( "  )\n")
print "%d Runs saved to goodRuns.txt"%counts
