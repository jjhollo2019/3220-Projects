/*
 * JBoss, the OpenSource J2EE webOS
 *
 * Distributable under LGPL license.
 * See terms of license at gnu.org.
 *
 */
package org.jboss.resource.adapter.jdbc;

import java.sql.SQLException;

/**
 * Does not check the exception
 *
 * @author <a href="mailto:adrian@jboss.org">Adrian Brock</a>
 * @version $Revision: 1.1.2.2 $
 */
public class NullExceptionSorter
   implements ExceptionSorter
{
   public boolean isExceptionFatal(SQLException e)
   {
      return false;
   }
}
