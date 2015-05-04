# Logger

## Contents of This File

- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation](#implementation)


## Introduction

Current maintainer: [whitehouse](https://www.drupal.org/u/whitehouse)

The Logger module provides additional logging capabailities, independent from
the Drupal logging backend. The module doesn't do anything on its own, but it
provides an API allowing developers to log system events with `logger_event()`
using [Graphite](http://graphite.readthedocs.org/)-compatible event names--i.e.,
paths delimited by dots (.). See [Getting Your Data Into Graphite: Step 1]
(http://graphite.readthedocs.org/en/latest/feeding-carbon.html#step-1-plan-a-naming-hierarchy)
.


## Requirements

No special requirements.


## Installation

Install as you would normally install a contributed Drupal module. See:
[Installing contributed modules]
(https://www.drupal.org/documentation/install/modules-themes/modules-8) for 
further information.


## Configuration

Enable or Disable debug mode for this module at Administration \> Configuration 
\> Development \> Logging and Errors.  When checked, all logger events are 
logged to watchdog.
