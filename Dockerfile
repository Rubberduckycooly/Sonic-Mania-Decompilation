FROM ubuntu:22.04 as build

# --- basic package installation ---
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
        git \
        libglew-dev \
        libglfw3-dev \
        libtheora-dev \
        portaudio19-dev

# --- TODO: stage submodules if not already ---
# RUN git submodule update --init, then do same for RSDKv5. But also make sure this command doesn't prevent image build from proceeding.

# --- set up work directory and env vars ---
RUN mkdir /sonic-mania-decomp
WORKDIR /sonic-mania-decomp

CMD echo 'Usage: docker run --rm -v ${PWD}:/Sonic-Mania-Decomp sonic-mania-decomp make\n'\
         'See https://github.com/Rubberduckycooly/Sonic-Mania-Decompilation/blob/master/README.md for more information'
