# Logger

## Contents of This File

- [Introduction](#introduction)
- [Installation](#installation)
- [Instrumentation](#instrumentation)
- [Logging Strategy](#logging-strategy)
- [Events](#events)


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
i.e., paths delimited by dots (`.`). See
[Getting Your Data Into Graphite: Step 1]
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

## Logging Strategy

While your module is free to adopt the logging strategy best suited to its
needs, the following example can be used as a starting point.  See
[Getting Your Data Into Graphite: Step 1]
(http://graphite.readthedocs.org/en/latest/feeding-carbon.html#step-1-plan-a-naming-hierarchy)
for more on naming events. 

1. In general, application events are given a name in the form 
   `events.{group}.{event_name}`, where `group` is the category to which the 
   event belongs. The group can be any string, but the general practice is to 
   use the name of the module calling `logger_event()`.
   
2. Exceptional events are given a log event name in the form
   `exceptions.{group}.{exception_id}`, where `group` is the category to which
   the event belongs and `exception_id` is a random, unique identifier for the 
   event, generated as follows:
   
   ```bash
   php -r 'print substr(sha1(time()), 0, 7) . "\n";'
   ```

   What constitutes an "exceptional event"? Any instance of application or
   infrastructure misbehavior that merits investigation when it occurs. This
   does not include bad user input or harmless anomalies. It includes such
   things as unexpected PDO exceptions, PHP exceptions, and the like. Exception
   `catch` clauses are a natural use, but not all such clauses should log and
   certainly they are not the only places to log. In general, whether or not an
   an event as an exception can be determined by asking the question, "Would I
   want to be notified if this was happening in production?" If yes, then log it
   as an exception. If no, then don't.

3. Metrics belonging to a process or data store are namespaced separately from
   other application events, but follow a similar naming convention. These 
   metrics are named `processes.{group}.{process_name}.{event_name}` and 
   `data_stores.{group}.{data_store_type}.{data_store_name}.{event_name}` 
   respectively.  See the [table of events](#events) below for examples.
   
   The Logger module includes two helper functions to log metrics related to 
   the start and end of a process, following the above *processes* naming pattern. 
   `logger_process_start()`, when called upon invocation of a process, will log
   the process's `invoked` counter. `logger_process_end()`, when called upon
   completion of the process, will log the process's `completed` counter, its
   `time_to_complete` timer and a counter for the returned `status`.

### Events

The strategy above results in the following list of events:

| Event | Type |
| --- | --- |
| `events.{group}.{event_name}` | count |
| `exceptions.{group}.{exception_id*}` | count |
| `processes.{group}.{process_name}.invoked` | count |
| `processes.{group}.{process_name}.completed` | count |
| `processes.{group}.{process_name}.time_to_complete` | time |
| `processes.{group}.{process_name}.statuses.{status}` | count |
| `data_stores.{group}.{data_store_type**}.{data_store_name}.count` | gauge |
| `data_stores.{group}.{data_store_type**}.{data_store_name}.item_added` | count |
| `data_stores.{group}.{data_store_type**}.{data_store_name}.item_removed` | count |

\* `exception_id`: e.g. "ad8sdf7"

\** `data_store_type`: e.g. "db", "queue", or "file".
