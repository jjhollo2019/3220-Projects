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
 * InformixExceptionSorter.java
 * 
 * This is a basic exception sorter for the IBM INFORMIX 9.4 RDBMS. All error
 * codes are taken from the IBM INFORMIX JDBC driver programmer's guide of the 
 * 2.21.JC6 JDBC driver. 
 *
 * @author <a href="mailto:u.schroeter@mobilcom.de">Ulf Schroeter</a>
 */
public class InformixExceptionSorter implements ExceptionSorter, Serializable
{

   public boolean isExceptionFatal(SQLException e)
   {
      switch (e.getErrorCode())
	  {
         case -79716: // System or internal error
         case -79730: // Connection noit established
         case -79734: // INFORMIXSERVER has to be specified
         case -79735: // Can't instantiate protocol
         case -79736: // No connection/statement established yet
         case -79756: // Invalid connection URL
         case -79757: // Invalid subprotocol
         case -79758: // Invalid IP address
         case -79759: // Invalid port nnumber
         case -79760: // Invalid database name
         case -79788: // User name must be specified
         case -79811: // Connection without user/password not supported
         case -79812: // User/password does not match with datasource
         case -79836: // Proxy error: no database connection
         case -79837: // Proxy error: IO error
         case -79879: // Unexpected exception
         
           return true;
      }

      return false;
   }

} // InformixExceptionSorter
