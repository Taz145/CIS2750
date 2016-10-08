#!/usr/bin/python3
import sys
import os
import re


def reader():
    try:
        os.mkfifo("q1")
    except OSError:
        pass
    text = []
    fifo = open("q1", "r")
    text.append(fifo.readline())
    text.append(fifo.read())
    fifo.close()
    os.unlink("q1")
    return text


def writer(text):
    fifo = open("q2", "w")
    for line in text:
        fifo.write(line)
    fifo.close()

def main(filename):
    text = reader();
    strInfo = []
    file = open(filename + ".info")
    for line in file:
        line = line.rstrip('\n')
        # add the tag and the target as a tuple to a list
        strInfo.append(tuple(line.split(" ", 1)))
    file.close()

    # sorts by longest strings so the tags don't mess up replacements later
    strInfo = sorted(strInfo, key=lambda tup: tup[1])
    title = text[0]
    body = text[1]
    for info in strInfo:
        replace = "<" + info[0] + ">" + info[1] + "</" + info[0] + ">"
        body = re.sub(r"\b" + re.escape(info[1]) + r"\b", replace, body)

    body = "<HTML>\n<HEAD>\n<TITLE>\n" \
        + title + "</TITLE>\n</HEAD>\n<BODY>\n" \
        + title + body + \
        "\n</BODY>\n</HTML>\n"
    writer(body)
    sys.exit(0)

if __name__ == '__main__':
    main(sys.argv[1])
