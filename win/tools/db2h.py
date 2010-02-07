#
# Copyright 2010, Harry Li <harry.li AT pagefreedom.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import sys

if len(sys.argv) < 3:
    print 'db2h.py input-file variable-name'
    sys.exit(0)

infile = sys.argv[1]
print 'Process '+infile+' ...'
cont = open(infile, 'rb').read(128*1024)
outfile = infile+'.h'
of = open(outfile, 'w')
of.write('')
of.write('')
of.write('const void* '+sys.argv[2]+' = ')
line=''
for i in range(len(cont)):
    line = line + '\\x%02X'%(ord(cont[i]))
    if i>0 and i%32==0:
        of.write('\"'+line+'\"\n\t')
        line=''
of.write(';\n')
of.write('#define '+sys.argv[2]+'_LEN '+str(len(cont))+'\n')
of.close()
print 'FIN'
