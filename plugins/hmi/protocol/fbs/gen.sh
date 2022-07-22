#!/bin/sh

flatc -c --gen-object-api *.fbs
mv *_generated.h ../generated
