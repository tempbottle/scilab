// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Han DONG
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Unitary tests for Scilab Java imagerenderer

// Exec tests common to all uicontrols
clear uicontrol_generic_test
exec("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
if uicontrol_generic_test("imagerenderer") <> 0 then pause; end

// Regular testing of imagerenderer in uicontrol
h = uicontrol("Style", "imagerenderer", "String", SCI + "/modules/demo_tools/images/logo_scilab.png", "position", [0 0 181 144]);
defaultValue = get(h, "value");
if or(defaultValue<>[1 1 0 0 0]) then pause; end
set(h, "value", [0.3 2 0 0 0]); // Change scale
set(h, "value", [0.3 2 0.1 2 0]); // Change shear
set(h, "value", [0.3 2 0.1 2 30]); // Change rotation angle

// TODO more test with wrong values
set(h, "value", [])
set(h, "value", [1])


// Test set functions for uicontrol
h = uicontrol("style", "imagerenderer");
set(h, "string", SCI + "/modules/demo_tools/images/logo_scilab.png");
set(h, "position", [0 0 300 300]);
set(h, "value", [0.3 2 0 0 0]);
set(h, "value", [0.3 2 0.1 2 0]);
set(h, "value", [0.3 2 0.1 2 30]);

h = uicontrol(gcf(), "style", "imagerenderer");
set(h, "string", SCI + "/modules/demo_tools/images/logo_scilab.png");
set(h, "position", [10 0 400 300]);
set(h, "value", [0.63 0.9 0 0 0]);
set(h, "value", [0.63 0.9 1 0.5 0]);
set(h, "value", [0.63 0.9 1 0.5 45]);

h = uicontrol("parent", gcf(), "style", "imagerenderer");
set(h, "string", SCI + "/modules/demo_tools/images/logo_scilab.png");
set(h, "position", [0 20 200 200]);
set(h, "value", [1.2 1.3 0 0 0]);
set(h, "value", [1.2 1.3 0.1 0.6 0]);
set(h, "value", [1.2 1.3 0.1 0.6 60]);