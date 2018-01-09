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

# add the bash profile so that the paths get set
#COPY bashrc /root/.bashrc

#RUN echo "IPW=/code/ipw-2.3.0" >> /.bash_profile
#RUN echo "export IPW" >> /.bash_profile
#RUN echo "PATH=`$IPW/path user`:$PATH" >> /.bash_profile
#RUN echo "MANPATH=`$IPW/path man`:$MANPATH" >> /.bash_profile
#RUN echo "export WORKDIR=/usr/local/tmp" >> /.bash_profile


# create the IPW environment variables for compiling
ENV IPW=/code/ipw
ENV WORKDIR=/tmp

# lets compile IPW
COPY . / /code/ipw/
RUN cd /code/ipw \
    && cp bashrc /root/.bashrc \
    && /bin/bash -c "source /root/.bashrc" \
    && ./configure \
    && make \
    && make install
    
ENTRYPOINT ["/bin/bash"]









