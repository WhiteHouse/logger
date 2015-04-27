# Logger

## Contents of This File

- [Introduction](#introduction)
- [Installation](#installation)
- [Instrumentation](#instrumentation)


## Introduction

Current maintainer: [whitehouse](https://www.drupal.org/u/whitehouse)

The Logger module provides event logging facilities that are decoupled from any
particular storage backend. It is useful for module developers who want to
instrument their code for monitoring and alerting without forcing their users
to use a particular backend technology, like StatsD. The module doesn't provide
any visible functionality out-of-the-box. You only need it if you're a module
developer looking to instrument your code or if a module you're using depends on
it.


## Installation

Logger itself is installed in the usual way. See [Installing contributed
modules](https://www.drupal.org/documentation/install/modules-themes/modules-7).
An implementation of `hook_logger_event()` is required to connect to a logging
backend so as to actually do something with event data. See [logger.api.php]
(logger.api.php). This implementation may be supplied in custom code or by a
contributed module like [StatsD](https://www.drupal.org/project/statsd).


## Instrumentation

Instrumentation is the process of adding "probes" to your application code to
fire events for Logger to log. If you're installing Logger as a dependency of
another module, presumably that module is already instrumented. To instrument
your own code, simply invoke `logger_event()`, much as you would [`watchdog()`]
(https://api.drupal.org/api/drupal/includes!bootstrap.inc/function/watchdog/7).
See [logger.module](logger.module).

If you're instrumenting a contributed module, you must [declare a dependency on
Logger in your .info file](https://www.drupal.org/node/542202#dependencies).
Alternatively, you can create a "soft", or optional, dependency on Logger by
using a wrapping function instead of calling `logger_event()` directly, like
this:

```php
function example_logger_event($name, $type = 'count', $value = 1) {
  if (function_exists('logger_event')) {
    logger_event($name, $type, $value);
  }
}
```

Event names should be [Graphite](http://graphite.readthedocs.org/)-compatible,
i.e., paths delimited by dots (`.`). See [Getting Your Data Into Graphite: Step 1]
(http://graphite.readthedocs.org/en/latest/feeding-carbon.html#step-1-plan-a-naming-hierarchy)
for some helpful advice.

Note: Logger has a debug mode that logs events to watchdog, which can be helpful
during development. It can be enabled at `admin/config/development/logging` or
by setting the `logger_debug` variable directly, e.g.:

```bash
# Enable debugging.
drush variable-set --exact logger_debug 1
# Disable debugging.
drush variable-delete --exact logger_debug
```
