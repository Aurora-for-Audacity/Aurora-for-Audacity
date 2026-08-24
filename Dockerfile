FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libgtk2.0-dev \
    build-essential \
    libcairo-dev \
    libx11-dev \
    uuid-dev \
    cmake \
    ninja-build \
    git \
    python3 \
    python3-pip \
    python3-venv \
    pkg-config \
    libexpat1-dev \
    libmp3lame-dev \
    libsndfile-dev \
    libasound2-dev \
    libsoxr-dev \
    libsqlite3-dev \
    libcurl4-openssl-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libid3tag0-dev \
    libogg-dev \
    libvorbis-dev \
    libflac-dev \
    libflac++-dev \
    liblilv-dev \
    lv2-dev \
    libserd-dev \
    libsord-dev \
    libsratom-dev \
    libsuil-dev \
    libportmidi-dev \
    libportsmf-dev \
    libsbsms-dev \
    libsoundtouch-dev \
    libtwolame-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Python virtual environment for Conan
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

RUN pip install --no-cache-dir --upgrade pip \
    && pip install --no-cache-dir conan

WORKDIR /src
COPY . .

# Sanity check
RUN cmake --version \
    && ninja --version \
    && python --version \
    && conan --version \
    && g++ --version
