# IPW is built on a ubuntu 18.04 image
FROM ubuntu:18.04

MAINTAINER Scott Havens <scott.havens@ars.usda.gov>

# create a working directory
RUN mkdir -p /code && mkdir -p /code/ipw

####################################################
# System requirements
####################################################

RUN echo 'Etc/UTC' > /etc/timezone \
    && ln -s /usr/share/zoneinfo/Etc/UTC /etc/localtime \
    && apt-get update -y \
    && apt-get install -y gcc make man \
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
    # && cp /code/ipw/bin/* /usr/local/bin/ \
    && apt-get autoremove -y gcc make
    
ENTRYPOINT ["/bin/bash"]









