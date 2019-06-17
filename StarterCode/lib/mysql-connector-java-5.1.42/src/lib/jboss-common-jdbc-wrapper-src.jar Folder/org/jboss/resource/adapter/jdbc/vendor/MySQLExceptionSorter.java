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
 * MySQLExceptionSorter.java
 * 
 * This is a basic exception sorter for the MySQL RDBMS. All error
 * codes are taken from the MySQL Connector Java 3.0.16 SQLError class. 
 *
 * @author <a href="mailto:u.schroeter@mobilcom.de">Ulf Schroeter</a>
 */
public class MySQLExceptionSorter implements ExceptionSorter, Serializable
{

   public boolean isExceptionFatal(SQLException e)
   {
      switch (e.getErrorCode())
	  {
      	// Communications Errors
	  	case 1040: // ER_CON_COUNT_ERROR
      	case 1042: // ER_BAD_HOST_ERROR
      	case 1043: // ER_HANDSHAKE_ERROR
      	case 1047: // ER_UNKNOWN_COM_ERROR
      	case 1081: // ER_IPSOCK_ERROR
      	case 1129: // ER_HOST_IS_BLOCKED
      	case 1130: // ER_HOST_NOT_PRIVILEGED
      		
        // Authentication Errors
      	case 1045: // ER_ACCESS_DENIED_ERROR
      		
        // Resource errors
      	case 1004: // ER_CANT_CREATE_FILE
      	case 1005: // ER_CANT_CREATE_TABLE
      	case 1015: // ER_CANT_LOCK
      	case 1021: // ER_DISK_FULL
      	case 1041: // ER_OUT_OF_RESOURCES
      		
        // Out-of-memory errors
      	case 1037: // ER_OUTOFMEMORY
      	case 1038: // ER_OUT_OF_SORTMEMORY
         
           return true;
      }

      return false;
   }

} // MySQLExceptionSorter
