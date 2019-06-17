/*
 * JBoss, the OpenSource J2EE webOS
 *
 * Distributable under LGPL license.
 * See terms of license at gnu.org.
 *
 */
package org.jboss.resource.adapter.jdbc;

import java.io.Serializable;
import java.sql.Connection;
import java.sql.SQLException;

/**
 * Does not check the connection
 *
 * @author <a href="mailto:adrian@jboss.org">Adrian Brock</a>
 * @version $Revision: 1.1.2.4 $
 */
public class NullValidConnectionChecker
   implements ValidConnectionChecker, Serializable
{
   private static final long serialVersionUID = -223752863430216887L;

   public SQLException isValidConnection(Connection c)
   {
      return null;
   }
}
