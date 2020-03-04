# IAB-Validator Library and Command-Line Application

## Introduction

The IAB-Validator library contains a suite of library components that allows developers to create client 
applications, products or modules for the validation of Immersive Audio Bitstream (IAB) files based on 
the SMPTE ST2098-2:2018 standard or its 2019 revision. It also includes a command-line application that 
is built on the library components for IAB validation against 6 constraint profiles (aka constraint sets).

The first published SMPTE IAB standard, ST 2098-2:2018, specifies an audio distribution format that 
supports both channel and object based audio. It is used to deliver immersive audio content for Digital 
Cinema (DC) applications. The standard was subsequently revised, published as ST 2098-2:2019, to support 
immersive audio content in Interoperable Master Format (IMF).

Validation and compliance to contraint profiles ensures IAB stream interoperability among tools and devices
with targeted profile(s) across toolchains and workflows.

## IAB constraint profiles

The IAB-Validator Library and Command-Line Application support IAB stream validation against 6 constraint 
profiles.

[SMPTE ST2098-2:2018](https://ieeexplore.ieee.org/document/8440792), the first published IAB standard, 
forms the *base profile* for validation against DC-targeted applications. Additionally, there are 2 more 
constraint profiles for DC applications, each with increased number of constraints. 
See [ToolHelp.md](./ToolHelp.md) for detail. 

[SMPTE ST2098-2:2019](https://ieeexplore.ieee.org/document/8752533), the 2019 revised IAB standard, forms 
the *base profile* for validation against IMF-targeted applications. Similarly, there are 2 additional 
constraint profiles for IMF applications, each with increased number of constraints. 
See [ToolHelp.md](./ToolHelp.md) for detail.

## How to build from source

CMake scripts are included for compiling and building libraries and command-line application. Refer to
[CMakeHelp.md](./CMakeHelp.md) for build steps and instructions.

## Libraries

* `IABValidator` - IABValidator library, for validating IAB frame or a group of consecutive frames belonging
to a single program
* `IABParser` - IABParser library, for parsing/reading serialized IAB stream into IAB frames
* `common-stream` - low level bitstream processing/serialize/de-serialize library. Dependency of IABParser 
library
* `DLC` - DLC decoder library for essence decoding. Dependency of IABParser library

## CLI Program

* `iab-validator` - a CLI application for IAB stream validation. See [ToolHelp.md](./ToolHelp.md) for how to run
iab-validator.

## License

The IAB-Validator Library is released under MIT-style [LICENSE](./LICENSE) by Xperi Corporation.

## Support and contact

* **E-mail:** [github.proaudio@xperi.com](mailto:github.proaudio@xperi.com)

