FROM alpine

ADD . /src
WORKDIR /src

RUN apk update
RUN apk add libtool sed gcc g++ make autoconf automake libtool
RUN ln -s /bin/sed /usr/bin/sed

RUN make
