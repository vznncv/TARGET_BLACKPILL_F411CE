# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2021-07-27
### Changed
- Make MbedOS v6.12.0 as minimal supported version.

### Added
- Add support of `mbed-tools`.

### Fixed
- Adjust target settings to use external clock oscillator.

## [0.1.4] - 2021-05-19
### Fixed

- Move `LED1` and `BUTTON1` from `PinName` enumeration to macros for compatibility with MbedOS v6.10.0.

## [0.1.3] - 2021-05-08
### Fixed

- Fixed compatibility with MbedOS v6.10.0.

## [0.1.2] - 2021-04-11
### Added

- Add github CI/CD to test project compilation with custom target.

### Fixed

- Fixed compatibility with MbedOS v6.9.0.

## [0.1.1] - 2021-02-09
### Fixed

- Add program uploading notes MbedOS v6.6.0.

## [0.1.0] - 2021-02-03

### Added

- Add BlackPill STM32F411CEU6 custom target support for Mbed OS 6.5
- Add base examples
