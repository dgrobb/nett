FROM ubuntu:21.10 as build

# Install the necessary build tools
RUN apt-get update && apt-get upgrade -y
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y g++ python3 git 
RUN apt-get install -y cmake ninja-build

# Get the correct version of the llvm repository
RUN cd /tmp && \
    git clone https://github.com/llvm/llvm-project.git && \
    cd llvm-project && \
    git checkout 82fbc5d45b0c2fc9050d1d5e335e35afb4ab2611
 
# Copy our source code into the correct subdirectory
ADD . /tmp/llvm-project/clang-tools-extra/nett

# Build the binary
RUN cd /tmp/llvm-project/clang-tools-extra/nett && ./build.sh

# Create the final image
FROM ubuntu:21.10
COPY --from=build /tmp/llvm-project/build/bin/nett /bin/nett
ENTRYPOINT ["/bin/nett"]
