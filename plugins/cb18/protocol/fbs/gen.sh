#!/bin/sh

flatc -c *.fbs
mv *_generated.h ../generated
