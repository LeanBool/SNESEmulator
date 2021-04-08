def readOpcodes(filename):
    aliases = {'#': 'IMM',
               'A': 'ACC',
               'r': 'REL',
               'rl': 'RELL',
               'I': 'IMP',
               's': 'STK',
               'd': 'DIR',
               'd,x': 'DIRX',
               'd,y': 'DIRY',
               '(d)': 'DIN',
               '(d,x)': 'DXI',
               '(d),y': 'DIX',
               '[d]': 'DIL',
               '[d],y': 'DIXL',
               'a': 'ABS',
               'a,x': 'ABX',
               'a,y': 'ABY',
               'al': 'ABL',
               'al,x': 'ABLX',
               'd,s': 'SREL',
               '(d,s),y': 'SRII',
               '(a)': 'ABI',
               '(a,x)': 'AII',
               'xyc': 'BLM'
               }
    
    out = []
    i = 0
    with open(filename, 'r') as f:
        for line in f.readlines():
            name, amode, cycles = line.split(" ")
            funcname = "&CPU::" + name
            amode = "&CPU::" + aliases[amode]
            print(i)
            out.append('{"%s", %s, %s, %s}' % (name, funcname, amode, cycles.replace("\n", "")))
            if i%16 == 0 and i != 0:
                out[i] = "\n" + out[i]
            i += 1
    return out

if __name__ == '__main__':
    formatted = ", ".join(readOpcodes("opcodeinfo.txt"))
    with open('formatted.txt', 'w') as f:
        f.write(formatted)
