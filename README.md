# Sequence checker
Language for sequences (directed graphs) definition and checker for it.

Author: Sergey Sobolev, sergey.sobolev@outlook.com

- [Sequence checker](#sequence-checker)
  - [Use cases](#use-cases)
  - [Build, test and run](#build-test-and-run)
    - [Prerequisites](#prerequisites)
    - [Build](#build)
    - [Test](#test)
    - [Generate code documentation](#generate-code-documentation)
    - [Run](#run)
      - [Note](#note)
  - [Sequence definition rules](#sequence-definition-rules)
    - [Supported constructions](#supported-constructions)
      - [Comments](#comments)
      - [Alternatives](#alternatives)
      - [Grouping](#grouping)
      - [Node parameter](#node-parameter)
      - [Complex sequences examples](#complex-sequences-examples)
        - [Note](#note-1)

## Use cases
This tool can be helpful if one wants to describe some expected behaviour and check reality against it.

E.g. there is some orchestrated process and certain sequence of calls that is supposed to happen, it can also have some variations, some of which are allowed, but some (all others) not.

So instead of manual analysis of system behaviour (e.g. megabytes or gigabytes of logs, or thousands of events in a message bus) it is possible to automate this process.

This tool can be integrated into some CI process for tests automation, also can be used for continuous monitoring support.

An integration scenario could look like this
1. define expected sequence of events
2. implement events extractor from some source(s) like message broker or syslog, the extractor should build sequences using the checker syntax.
3. trigger the tool and check the output

## Build, test and run

### Prerequisites

Sequence checker is a pure ANSI C tool, it can be built on any platform which has C compiler. Tested on Windows and various Linux distributives. 

- On Windows can be built using MS Visual Studio on MingGW GCC
- On Linux - GCC
- make tool

Dependencies
- bison package (parsers generator), can also use yacc

Code documentation generation
- doxygen

### Build

There are two options
1. `make all` - will build the checker only
2. `make build` - will build the application and run unit tests

To re-build the checker use the clean target: `make clean`

### Test

One can run unit tests in the **out** folder by invoking the check target: ```make check```. 
The result status (in Linux) can be observed by running ```echo $?``` (check the last command return code). If result is 0, then no errors, 1 otherwise.

Note, some tests use sample.txt file in the **out** folder, it must contain valid definitions.

### Generate code documentation
1. Go to the docs folder and run `make all` or `doxygen doxyfile`
2. Open the file html/index.html in your browser

### Run

The sequence checker understands the following arguments:
* --version - will return version of the tool
* --? - ultra short help
* -v - verbose output
* -vv - very verbose output
* --cfg - configuration file (will be parsed before checking sequence), e.g. one can split definitions of nodes and paths
* \<file name\> - input file with sequences to be checked

Alternatively if no file specified sequences can be taken from standard input. To end input on keyboard press Ctrl-D, then check the command execution status (`echo $?`) as with the tests.

These two commands (in Linux) will produce the same result:
- ./sc_checker sample.txt
- cat sample.txt | ./sq_checker

#### Note

Input files can contain multiple sequences, if any of checks will fail, then output status will be 1 (faulted). If output status is 0, then it means that no errors have been found in any of defined sequences.

## Sequence definition rules

The checker operates based on language grammar specified in the file *src/sc_grammar.y*

### Supported constructions

- Node name definition (can be a state, service name, etc.):
`def NODE_NAME;` - def - keyword, NODE_NAME - string, semicolon - required delimiter

- Allowed sequence (path) definition:<br>
  `allowed=<path>->#;`

- Executed sequence (path) definition:<br>
  `executed=<path>->#;`

Where <path> contains sequence of known nodes with optional parameters and variations. The ending `->#` terminates the sequence and instructs the checker that there must be no further nodes in the sequence. Semicolon is a mandatory delimiter of the definition.

The simplest case: <br>
`allowed=A->#;` <br>
`executed=A->#;` <br>

A valid sequence. The next one - not <br>
`allowed=A->#;` <br>
`executed=A->A->#;` <br>
because `A->A` transition has not been allowed

#### Comments

It is possible to add comments: everything after double slashes will be ignored by the checker:
```
// this is a comment
def A; // this is an inline comment, also fine
```

#### Alternatives

The checker supports alternatives definitions in sequences using the pipe character:
```
def A;
def B;

allowed=A->A|B->#; // note the | between A and B
executed=A->A->#; // valid
executed=A->B->#; // also valid
executed=A->B->A->#; // invalid, as no such transition allowed
```

#### Grouping

Nodes can be grouped into subsequences (subpaths). It is useful for more complex alternatives specification, e.g.

```
def A;
def B;
def C;

allowed=A->B|[C->B]->#; // note the [] brackets
executed=A->B->#; // valid
executed=A->C->B->#; // also valid
executed=A->B->C->B->#; // invalid, as no such transitions allowed
```

#### Node parameter

The checker supports optional node parametrisation:
```
def A;

allowed=A(10)->#; // note the (10)
```

It has a special logic: the executed node must have *non-negative* parameter value less than the parameter (limit):

```
executed=A(-1)->#; // syntax error
executed=A(0)->#;  // valid sequence
executed=A(9)->#;  // still valid sequence
executed=A(10)->#; // already invalid: param equals the limit
```

#### Complex sequences examples

```
def A;
def B;
def C;
def D;
def E;

allowed=A->[B(1)->C]|[B(2)->D]->E->#;
executed=A->B(0)->C->E->#; // valid
executed=A->B(1)->D->E->#; // valid
executed=A->B(1)->C->E->#; // invalid

allowed=A->[B(1)->C]|[B(2)->D->E]->#; // E is now part of the second group
executed=A->B(0)->C->#; // valid
executed=A->B(1)->D->E->#; // valid, because 1<2 and the second alternative of the allowed sequence gets used
executed=A->B(0)->C->E->#; // invalid
```

##### Note
With alternatives and parametrisation it is a bit tricky though, because these alternatives are not commutative, e.g. if we swap the group then the results will be different:

```
def A;
def B;
def C;
def D;
def E;

allowed=A->[B(1)->C]|[B(2)->D]->E->#;
executed=A->B(0)->C->E->#; // valid
executed=A->B(1)->D->E->#; // valid
executed=A->B(1)->C->E->#; // invalid

allowed=A->[B(2)->D->E]|[B(1)->C]->#; // swapped the group from the previous example
executed=A->B(0)->C->#; // invalid - the first group gets triggered as 0<2 and the sequence does not allow this transition
executed=A->B(1)->D->E->#; // valid
executed=A->B(0)->D->E->#; // valid
```

Basically, the second group `B(1)->C` will never be allowed as B(2) node specification fully covers all possible values for the both groups.

