/*
 * JBoss, the OpenSource J2EE webOS
 *
 * Distributable under LGPL license.
 * See terms of license at gnu.org.
 *
 */
package org.jboss.resource.adapter.jdbc;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.Modifier;
import java.security.PrivilegedExceptionAction;
import java.security.AccessController;
import java.security.PrivilegedActionException;
import java.util.HashSet;

/** A PrivilegedExceptionAction that proxies whatever interfaces the target
 * object supports. When an invocation through an interface method is done,
 * the is executed via reflection in a PrivilegedExceptionAction to run
 * at the privilege level of this codebase.
 * 
 * @author Scott.Stark@jboss.org
 * @version $Revison:$
 */
public class PrivilegedProxy
   implements InvocationHandler, PrivilegedExceptionAction
{
   private Object target;
   private Method method;
   private Object[] args;

   public Object run() throws Exception
   {
      Object value = null;
      try
      {
         value = method.invoke(target, args);
      }
      catch(InvocationTargetException e)
      {
         throw (Exception) e.getTargetException();
      }
      return value;
   }

   PrivilegedProxy(Object target)
   {
      this.target = target;
   }

   Object getProxy() throws Exception
   {
      BuildProxyAction action = new BuildProxyAction(target, this);
      Object proxy = null;
      try
      {
         proxy = AccessController.doPrivileged(action);
      }
      catch(PrivilegedActionException e)
      {
         throw e.getException();
      }
      return proxy;
   }

   public Object invoke(Object proxy, Method method, Object[] args)
      throws Throwable
   {
      this.method = method;
      this.args = args;
      Object value = null;
      try
      {
         value = AccessController.doPrivileged(this);
      }
      catch(PrivilegedActionException e)
      {
         throw e.getException();
      }
      return value;
   }

   private static class BuildProxyAction implements PrivilegedExceptionAction
   {
      Object target;
      InvocationHandler handler;
      BuildProxyAction(Object target, InvocationHandler handler)
      {
         this.target = target;
         this.handler = handler;
      }
      public Object run() throws Exception
      {
         HashSet tmp = new HashSet();
         // Get all interfaces
         Class c = target.getClass();
         do
         {
            Class[] ifaces = c.getInterfaces();
            for(int i = 0; i < ifaces.length; i ++)
            {
               Class iface = ifaces[i];
               int modifiers = iface.getModifiers();
               if( Modifier.isPublic(modifiers) )
                  tmp.add(iface);
            }
            c = c.getSuperclass();
         } while( c != null );

         Class[] ifaces = new Class[tmp.size()];
         tmp.toArray(ifaces);
         ClassLoader loader = Thread.currentThread().getContextClassLoader();
         return Proxy.newProxyInstance(loader, ifaces, handler);
      }
   }
}
