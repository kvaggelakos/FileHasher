#!/bin/bash

echo "Cleaning database..."
mysql -uroot -proot < basic.sql

echo "Done!"

