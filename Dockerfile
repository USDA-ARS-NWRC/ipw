# IPW is built on a ubuntu 17.10 image
FROM ubuntu:18.04

MAINTAINER Scott Havens <scott.havens@ars.usda.gov>

# create a working directory
RUN mkdir -p /code && mkdir -p /code/ipw

####################################################
# System requirements
####################################################

RUN apt-get update -y \
    && apt-get install -y build-essential man \
    && rm -rf /var/lib/apt/lists/*

####################################################
# IPW
####################################################

# create the IPW environment variables for compiling
ENV IPW=/code/ipw
#ENV WORKDIR=/tmp

# lets compile IPW
COPY . / /code/ipw/

RUN cd /code/ipw \
    && cp bashrc /root/.bashrc \
    && /bin/bash -c "source /root/.bashrc" \
    && ./configure \
    && make \
    && make install \
    && cp /code/ipw/bin/* /usr/local/bin/
    
ENTRYPOINT ["/bin/bash"]









