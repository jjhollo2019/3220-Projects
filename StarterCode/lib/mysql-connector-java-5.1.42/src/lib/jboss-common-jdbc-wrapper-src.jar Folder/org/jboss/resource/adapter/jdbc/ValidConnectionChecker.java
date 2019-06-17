/*
 * JBoss, the OpenSource J2EE webOS
 *
 * Distributable under LGPL license.
 * See terms of license at gnu.org.
 *
 */
package org.jboss.resource.adapter.jdbc;

import java.sql.Connection;
import java.sql.SQLException;

/**
 * Checks that a connection is valid
 *
 * @author <a href="mailto:adrian@jboss.org">Adrian Brock</a>
 * @version $Revision: 1.1.2.2 $
 */
public interface ValidConnectionChecker
{
   /**
    * Checks the connection is valid
    *
    * @param c the connection
    * @return Exception when not valid, null when valid
    */
   SQLException isValidConnection(Connection c);
}
