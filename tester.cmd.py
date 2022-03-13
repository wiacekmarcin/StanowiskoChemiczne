import sys
import crc8


while True:
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Param\n\t3. Position\n\t4. Echo\n\t5. MoveHome\n\t6. Reset\n\n\t0 - Wyjscie\n[1-5]:')
    hash = crc8.crc8()
    if i =='0':
        sys.exit(0)
    if i == '1':
        hash.update("%c" % 0x10)
        c = hash.hexdigest()
        print ("10%s" % c)
    if i == '2':
        hash = crc8.crc8()
        step = int(raw_input("Podaj maksymalna ilosc krokow: "))
        s = "%c%c%c%c%c%c" % (0x35, 0x00, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff)
        crc = hash.update(s)
        c = hash.hexdigest()
        print ("3500%02x%02x%02x%02x%s" % (step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, c))
    if i == '3':
        hash = crc8.crc8()
        dozo = int(raw_input("Podaj nr dozownika: "))

        step = int(raw_input("Podaj ilosc krokow: "))
        s = "%c%c%c%c%c%c" % (0x55, dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff)
        crc = hash.update(s)
        c = hash.hexdigest()
        print ("55%02x%02x%02x%02x%02x%s" % (dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, c))
    if i == '4':
        hash.update("%c" % 0x70)
        c = hash.hexdigest()
        print ("70%s" % c)
    if i == '5':
        dozo = int(raw_input("Podaj nr dozownika: "))
        hash.update("%c%c" % (0x91, dozo))
        c = hash.hexdigest()
        print ("91%02x%s" % (dozo, c))
    if i == '6':
        hash.update("%c" % 0xb0)
        c = hash.hexdigest()
        print ("b0%s" % c)