/*
 * JBoss, the OpenSource J2EE webOS
 *
 * Distributable under LGPL license.
 * See terms of license at gnu.org.
 *
 */
package org.jboss.resource.adapter.jdbc.vendor;

import java.io.Serializable;
import java.sql.SQLException;
import org.jboss.resource.adapter.jdbc.ExceptionSorter;


/**
 * OracleExceptionSorter.java
 *
 *
 * Created: Fri Mar 14 21:54:23 2003
 *
 * @author <a href="mailto:an_test@mail.ru">Andrey Demchenko</a>
 * @author <a href="mailto:d_jencks@users.sourceforge.net">David Jencks</a>
 */
public class OracleExceptionSorter implements ExceptionSorter, Serializable
{

   public boolean isExceptionFatal(SQLException e)
   {
      switch (e.getErrorCode()) {
         case    28: // Session has been killed
         case   107: // Failed to connect to ORACLE listener process
         case   600: // Internal oracle error
         case  1012: // Not logged on
         case  1014: // Oracle shutdown in progress
         case  1033: // Oracle initialization or shutdown in progress
         case  1034: // Oracle not available
         case  2396: // Exceeded maximum idle time, please connect again
         case  3111: // Break received on communication channel
         case  3113: // End-of-file on communication channel
         case  3114: // Not connected to ORACLE
         case 12541: // TNS:no listener, etc.
           // JDBC Error Messages
         case 17002: // Io Exception
         case 17008: // Closed Connection
         case 17027: // Stream has already been closed
           return true;
      }

      String errorText = e.getMessage().toUpperCase();
      return (errorText.indexOf("TNS") > -1)   // TNS Net8 messages
         || (errorText.indexOf("SOCKET") > -1)  // Control socket error
         || (errorText.indexOf("BROKEN PIPE") > -1);

   }

} // OracleExceptionSorter
