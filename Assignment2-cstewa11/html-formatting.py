#!/usr/bin/python3
import sys
import os
import re


def reader():
    try:
        os.mkfifo("q1")
    # ignore exception if the pipe already exists
    except FileExistsError:
        pass
    text = []
    fifo = open("q1", "r")
    text.append(fifo.readline())
    for line in fifo:
        text.append(line)
    fifo.close()
    os.unlink("q1")
    return text


def writer(text, filename):
    fifo = open("q2", "w")
    fifo.write(text)
    fifo.close()

    html = open(filename +".html", "w")
    html.write(text)
    html.close()


def main(filename):
    text = reader()
    title = text[0]
    strInfo = []

    try:
        file = open(filename + ".info")
        # If file can't be found continue without style formatting
    except FileNotFoundError:
        print(filename + ".info couldn't be found.\n")
    else:
        for line in file:
            line = line.rstrip('\n')
            # add the tag and the target as a tuple to a list
            strInfo.append(tuple(line.split(" ", 1)))
        file.close()

        # sorts by longest strings so the tags don't mess up replacements later
        strInfo = sorted(strInfo, key=lambda tup: tup[1])
        for info in strInfo:
            replace = "<" + info[0] + ">" + info[1] + "</" + info[0] + ">"
            text[1]  = re.sub(re.escape(info[1]) + r"\b", replace, text[1])

    body = text[1]
    body = "<HTML>\n<HEAD>\n<TITLE>\n" \
        + title + "</TITLE>\n</HEAD>\n<BODY>\n" \
        + title + body + \
        "\n</BODY>\n</HTML>\n"
    writer(body, filename)


if __name__ == '__main__':
    main(sys.argv[1])

