cmake_minimum_required(VERSION 3.6)

set(FAKE_DBS scripts/dhcpd.db)

find_path()

file(COPY ${FAKE_DBS} DESTINATION databases)