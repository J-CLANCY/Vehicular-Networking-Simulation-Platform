#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2020 German Aerospace Center (DLR) and others.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# https://www.eclipse.org/legal/epl-2.0/
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the Eclipse
# Public License 2.0 are satisfied: GNU General Public License, version 2
# or later which is available at
# https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2016-11-25

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# apply zoom
netedit.setZoom("25", "25", "25")

# go to additional mode
netedit.additionalMode()

# select E3
netedit.changeElement("e3Detector")

# create E3
netedit.leftClick(referencePosition, 250, 100)

# create second E3
netedit.leftClick(referencePosition, 400, 100)

# select entry detector
netedit.changeElement("detEntry")

# Create Entry detector for both E3s
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 180, 240)
netedit.leftClick(referencePosition, 400, 100)
netedit.leftClick(referencePosition, 180, 300)

# Create Exit detector for both E3s
netedit.changeElement("detExit")
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 400, 240)
netedit.leftClick(referencePosition, 400, 100)
netedit.leftClick(referencePosition, 400, 300)

# go to inspect mode
netedit.inspectMode()

# inspect Entry
netedit.leftClick(referencePosition, 200, 240)

# Change Netedit parameter 1 with a non valid value (Invalid E3 ID)
netedit.modifyAttribute(6, "invalidE3", True)

# Change Netedit parameter 2 with a non valid value (Invalid E3 ID)
netedit.modifyAttribute(6, "e3Detector_1", True)

# Check undos and redos
netedit.undo(referencePosition, 7)
netedit.redo(referencePosition, 7)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)