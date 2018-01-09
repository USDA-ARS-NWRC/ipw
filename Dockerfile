# IPW is built on a ubuntu 16.04 image
FROM ubuntu:16.04

MAINTAINER Scott Havens <scott.havens@ars.usda.gov>

# create a working directory
RUN mkdir -p /code && mkdir -p /code/ipw

####################################################
# System requirements
####################################################

RUN apt-get update -y && \
    apt-get install -y build-essential \
    man

####################################################
# IPW
####################################################

# create the IPW environment variables for compiling
ENV IPW=/code/ipw
ENV WORKDIR=/tmp

# lets compile IPW
#COPY . / /code/ipw/
COPY /ipw /code/ipw
RUN cd /code/ipw \
    && cp bashrc /root/.bashrc \
    && /bin/bash -c "source /root/.bashrc" \
    && ./configure \
    && make \
    && make install
    
ENTRYPOINT ["/bin/bash"]









