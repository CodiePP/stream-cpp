# stream-cpp

simple streaming library with two modes:

- `push` a processed buffer to a target stream
- `pull` a buffer from a source stream and process

## dependency

this builds on the package [sizebounded](https://github.com/CodiePP/sizebounded).

## implementation

derive from class `stream` and provide an implementation of the function:

```
virtual sizebounded<T,sz>& process(sizebounded<T,sz>&) const
```
