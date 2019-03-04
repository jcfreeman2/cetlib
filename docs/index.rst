.. cetlib documentation master file, created by
   sphinx-quickstart on Sun Jul  8 22:29:32 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

*cetlib* library
================

.. toctree::
   :maxdepth: 2

   Release notes<releaseNotes>

The *cetlib* C++ library serves as a home for numerous classes and
functions of general applicability.  While these components are not
tied to any specific project, many of them did originate as part of
some other project, and were factored out of their original home when
it was recognized they had broader utility than originally envisioned.

Other than their broad applicability, there is no common theme
unifying these tools.  However, it is envisioned that as *cetlib*
evolves, one or more selected subsets of these components may in
future be factored out of *cetlib* into their own libraries.

test - ryan

.. note:: A subset of the *cetlib* library is the *cetlib_except*
   library, which includes exception utilities and a symbol demangler.

Containers
----------

* :rsource:`cetlib/container_algorithms.h`: Function templates wrapping standard algorithms.
* :rsource:`cetlib/map_vector.h`: A map-like data structure mimicking a sparse vector interface.
* :rsource:`cetlib/registry.h`: A singleton ``std::map<K,V>``.
* :rsource:`cetlib/registry_via_id.h`: A singleton ``std::map<K,V>`` requiring that ``V::id()`` exists and yields values of type ``K``.



Environment and files
---------------------

* :rsource:`cetlib/filepath_maker.h`: A family of related policies governing the translation of a filename into a fully-qualified filepath.
* :rsource:`cetlib/filesystem.h`: A family of file-level utilities.
* :rsource:`cetlib/getenv.h`: Obtain value of environment variable; throw if none such.
* :rsource:`cetlib/include.h`: File->string, interpolating ``#include``'d files along the way.
* :rsource:`cetlib/includer.h`: A container-like data structure that (a) transparently handles #include'd files, and (b) can trace back its iterators.
* :rsource:`cetlib/search_path.h`: Seek filename or pattern in a given list of pathnames.
* :rsource:`cetlib/shlib_utils.h`: Obtain the shared library prefix and suffix for the current system.
* :rsource:`cetlib/split_path.h`: Split the string 'path' into components delimited by a single colon. Adjacent colons result in an empty string.


Hashing
-------

* :rsource:`cetlib/crc32.h`: Calculate a CRC32 checksum.
* :rsource:`cetlib/MD5Digest.h`: Secure Hash Algorithm as defined in RFC 1321, 1992-04.
* :rsource:`cetlib/sha1.h`: Secure Hash Algorithm as defined in FIPS PUB 180-1, 1993-05-11.

Library and plugin management
-----------------------------

* :rsource:`cetlib/BasicPluginFactory.h`: Refinement of PluginFactory for a common class of plugins featuring a "pluginType" function and a "makePlugin" function.
* :rsource:`cetlib/LibraryManager.h`: Facilities for managing libraries matching a particular name convention, loading and obtaining symbols from  same.
* :rsource:`cetlib/PluginFactory.h`: General facility for managing the loading and creation of plugins.
* :rsource:`cetlib/PluginTypeDeducer.h`: Function template that is helpful with plugins following the BasicPluginFactory paradigm. Intended to be specialized as required.

Multithreading
--------------

* :rsource:`cetlib/assert_only_one_thread.h`: Macro used to assert that only one thread accesses the given code block at a time.
* :rsource:`cetlib/SimultaneousFunctionSpawner.h`: Utility to test the behavior of a function when it is spawned via multiple threads.

Numerics and values
-------------------

* :rsource:`cetlib/bit_manipulation.h`: Compile-time bit manipulations.
* :rsource:`cetlib/hypot.h`: Checked and unchecked Euclidean planar hypotenuse calculations.
* :rsource:`cetlib/nybbler.h`: Half-byte manipulations.
* :rsource:`cetlib/pow.h`: Minimal-multiply algorithm for integral non-negative powers.
* :rsource:`cetlib/simple_stats.h`: Simple statistics.
* :rsource:`cetlib/zero_init.h`: Wrap a value of native arithmetic or pointer type T, ensuring a default-initialized value of T(0) (obsoleted by uniform initialization syntax in C++11).

Pointers and references
-----------------------

* :rsource:`cetlib/exempt_ptr.h`: A pointer taking no notice of its pointee's lifetime (thus exempt from any role in managing same, and thus copyable with no impact on its pointee).
* :rsource:`cetlib/hard_cast.h`: Provide unchecked, unconditional casting of one pointer type to another. Only for use when a C++ ``{dynamic,static,reinterpret}_cast`` is not sufficient to the task. such as casting a ``void*`` to pointer-to-function.
* :rsource:`cetlib/maybe_ref.h`: Reference-like types that may refer to nothing.
* :rsource:`cetlib/make_unique.h`: Provide a function to make a std::unique_ptr (obsoleted by C++14).
* :rsource:`cetlib/ostream_handle.h`: Properly handle ownership for std::ostream objects (e.g. std::cout vs. std::ofstream)
* :rsource:`cetlib/value_ptr.h`: A pointer treating its pointee as-if contained by value.

SQLite utilities
----------------

* :rsource:`cetlib/sqlite/column.h`: Type-safe C++ interface used when creating a column of an SQLite table.
* :rsource:`cetlib/sqlite/ConnectionFactory.h`: Thread-safe factory which is used to ensure consistent database-locking for a given SQLite database connection.
* :rsource:`cetlib/sqlite/Connection.h`: Thread-aware handle to an SQLite database.
* :rsource:`cetlib/sqlite/create_table.h`: Type-safe C++ interface for creating an SQLite table.
* :rsource:`cetlib/sqlite/exec.h`: String-based C++ interface for executing SQLite statements.
* :rsource:`cetlib/sqlite/insert.h`: Type-safe C++ interface for inserting values into the row of an SQLite table.
* :rsource:`cetlib/sqlite/Ntuple.h`: Class template that provides thread-safe insertion into an SQLite table.
* :rsource:`cetlib/sqlite/query_result.h`: The return type when using the cet::sqlite::select functionality.
* :rsource:`cetlib/sqlite/select.h`: Type-safe C++ interface for querying an SQLite table.
* :rsource:`cetlib/sqlite/statistics.h`: Basic functions that provide statistics information based on the values of a given column.
* :rsource:`cetlib/sqlite/Transaction.h`: C++ wrapper that provides SQLite transaction facilities.

String manipulation
-------------------

* :rsource:`cetlib/base_converter.h`: Convert a number string to or from a particular radix.
* :rsource:`cetlib/canonical_number.h`: Transform a number string into a canonical form.
* :rsource:`cetlib/canonical_string.h`: Transform a string into a canonical form.
* :rsource:`cetlib/column_width.h`: Obtain the length of the longest in a vector of strings.
* :rsource:`cetlib/lpad.h`: Left-pad a string to the wanted size, unless already longer.
* :rsource:`cetlib/ntos.h`: Produce a string from a number like std::to_string, but also supporting larger numeric types (if any).
* :rsource:`cetlib/rpad.h`: Right-pad a string to the wanted size, unless already longer.
* :rsource:`cetlib/split.h`: Obtain substrings at a string's specified boundaries.
* :rsource:`cetlib/ston.h`: Produce a number from a string like std::stod and friends, but also supporting larger numeric types (if any).
* :rsource:`cetlib/trim.h`: Remove a string's leading/trailing whitespace.

Uncategorized
-------------

* :rsource:`cetlib/cpu_timer.h`: Measure the cpu and wallclock elapsed times.
* :rsource:`cetlib/name_of.h`: return a string representation of any type (may specialize).
* :rsource:`cetlib/no_delete.h`: do-nothing deleter policy that can be used with smart pointers.
