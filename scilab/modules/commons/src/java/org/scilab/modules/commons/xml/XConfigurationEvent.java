/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import java.util.Set;

public class XConfigurationEvent {

    private Set<String> changedPaths;

    /**
     * Default constructor
     * @param changedPaths a set containing all the paths which have been modified
     */
    public XConfigurationEvent(Set<String> changedPaths) {
        this.changedPaths = changedPaths;
    }

    /**
     * Get the modified paths
     * @return the modified paths as a set
     */
    public Set<String> getModifiedPaths() {
        return changedPaths;
    }
}