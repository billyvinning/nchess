nchess
======

Chess in the terminal, written in C99.

Currently, this project is only capable of player vs. player play in the same terminal; though there are future plans for human vs. computer support.

Build
-----

The only dependency is ``ncurses``. To build the package with GCC 12.2.1, execute the following:

.. code-block:: console

    make clean && make


Contributing
------------

The dependencies are ``clang-format``, ``clang-tidy`` and ``include-what-you-use``; to install the pre-commit hooks, execute the following:

.. code-block:: console

    pre-commit install


License
-------

This project is licensed under the GPLv3 license. Please see ``COPYING.rst`` for details.

