/***************************************************************************
    qgsapplication.h - Accessors for application-wide data
     --------------------------------------
    Date                 : 02-Jan-2006
    Copyright            : (C) 2006 by Tom Elwertowski
    Email                : telwertowski at users dot sourceforge dot net
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSAPPLICATION_H
#define QGSAPPLICATION_H

#include <QApplication>
#include <QEvent>
#include <QStringList>

#include <qgis.h>
#include <qgsconfig.h>

/** \ingroup core
 * Extends QApplication to provide access to QGIS specific resources such
 * as theme paths, database paths etc.
 */

#ifdef ANDROID
typedef void XEvent;
#endif

class CORE_EXPORT QgsApplication : public QApplication
{
    Q_OBJECT
  public:
    static const char* QGIS_ORGANIZATION_NAME;
    static const char* QGIS_ORGANIZATION_DOMAIN;
    static const char* QGIS_APPLICATION_NAME;
    QgsApplication( int & argc, char ** argv, bool GUIenabled, QString customConfigPath = QString() );
    virtual ~QgsApplication();

    /** This method initialises paths etc for QGIS. Called by the ctor or call it manually
        when your app does not extend the QApplication class.
        @note you will probably want to call initQgis too to load the providers in
        the above case.
        */
    static void init( QString customConfigPath = QString() );

    //! Watch for QFileOpenEvent.
    virtual bool event( QEvent * event ) override;

    //! Catch exceptions when sending event to receiver.
    virtual bool notify( QObject * receiver, QEvent * event ) override;

    //! Set the FileOpen event receiver
    static void setFileOpenEventReceiver( QObject * receiver );

    /** Set the active theme to the specified theme.
     * The theme name should be a single word e.g. 'default','classic'.
     * The theme search path usually will be pkgDataPath + "/themes/" + themName + "/"
     * but plugin writers etc can use themeName() as a basis for searching
     * for resources in their own datastores e.g. a Qt4 resource bundle.
     * @note A basic test will be carried out to ensure the theme search path
     * based on the supplied theme name exists. If it does not the theme name will
     * be reverted to 'default'.
     */
    static void setThemeName( const QString &theThemeName );

    /** Set the active theme to the specified theme.
     * The theme name should be a single word e.g. 'default','classic'.
     * The theme search path usually will be pkgDataPath + "/themes/" + themName + "/"
     * but plugin writers etc can use this method as a basis for searching
     * for resources in their own datastores e.g. a Qt4 resource bundle.
     */
    static const QString themeName();

    //! Returns the path to the authors file.
    static const QString authorsFilePath();

    /** Returns the path to the contributors file.
     * Contributors are people who have submitted patches
     * but don't have commit access. */
    static const QString contributorsFilePath();

    /** Returns the path to the developers map file.
     * The developers map was created by using leaflet framework,
     * it shows the doc/contributors.json file.
     * @note this function was added in version 2.7 */
    static const QString developersMapFilePath();

    /** Returns the path to the sponsors file. */
    static const QString sponsorsFilePath();

    /** Returns the path to the donors file. */
    static const QString donorsFilePath();

    /**
     * Returns the path to the sponsors file.
     */
    static const QString translatorsFilePath();

    /*!
      Returns the path to the licence file.
    */
    static const QString licenceFilePath();

    //! Returns the path to the help application.
    static const QString helpAppPath();

    //! Returns the path to the translation directory.
    static const QString i18nPath();

    //! Returns the path to the master qgis.db file.
    static const QString qgisMasterDbFilePath();

    //! Returns the path to the settings directory in user's home dir
    static const QString qgisSettingsDirPath();

    //! Returns the path to the user qgis.db file.
    static const QString qgisUserDbFilePath();

    //! Returns the path to the splash screen image directory.
    static const QString splashPath();

    //! Returns the path to the icons image directory.
    static const QString iconsPath();

    //! Returns the path to the srs.db file.
    static const QString srsDbFilePath();

    //! Returns the pathes to svg directories.
    static const QStringList svgPaths();

    //! Returns the system environment variables passed to application.
    static const QMap<QString, QString> systemEnvVars() { return ABISYM( mSystemEnvVars ); }

    //! Returns the path to the application prefix directory.
    static const QString prefixPath();

    //! Returns the path to the application plugin directory.
    static const QString pluginPath();

    //! Returns the common root path of all application data directories.
    static const QString pkgDataPath();

    //! Returns the path to the currently active theme directory.
    static const QString activeThemePath();

    //! Returns the path to the default theme directory.
    static const QString defaultThemePath();

    //! Returns path to the desired icon file.
    //! First it tries to use the active theme path, then default theme path
    static QString iconPath( QString iconFile );

    //! Helper to get a theme icon. It will fall back to the
    //! default theme if the active theme does not have the required icon.
    static QIcon getThemeIcon( const QString &theName );

    //! Helper to get a theme icon as a pixmap. It will fall back to the
    //! default theme if the active theme does not have the required icon.
    static QPixmap getThemePixmap( const QString &theName );

    //! Returns the path to user's style.
    static const QString userStyleV2Path();

    //! Returns the path to default style (works as a starting point).
    static const QString defaultStyleV2Path();

    //! Returns the path containing qgis_core, qgis_gui, qgispython (and other) libraries
    static const QString libraryPath();

    //! Returns the path with utility executables (help viewer, crssync, ...)
    static const QString libexecPath();

    //! Alters prefix path - used by 3rd party apps
    static void setPrefixPath( const QString &thePrefixPath, bool useDefaultPaths = false );

    //! Alters plugin path - used by 3rd party apps
    static void setPluginPath( const QString &thePluginPath );

    //! Alters pkg data path - used by 3rd party apps
    static void setPkgDataPath( const QString &thePkgDataPath );

    //! Alters default svg paths - used by 3rd party apps.
    static void setDefaultSvgPaths( const QStringList& pathList );

    //! loads providers
    static void initQgis();

    //! initialise qgis.db
    static bool createDB( QString* errorMessage = 0 );

    //! deletes provider registry and map layer registry
    static void exitQgis();

    /** constants for endian-ness */
    typedef enum ENDIAN
    {
      XDR = 0,  // network, or big-endian, byte order
      NDR = 1   // little-endian byte order
    }
    endian_t;

    //! Returns whether this machine uses big or little endian
    static endian_t endian();

    //! Swap the endianness of the specified value
    template<typename T>
    static void endian_swap( T& value )
    {
      char* data = reinterpret_cast<char*>( &value );
      std::size_t n = sizeof( value );
      for ( std::size_t i = 0, m = n / 2; i < m; ++i )
      {
        std::swap( data[i], data[n - 1 - i] );
      }
    }

    /** \brief get a standard css style sheet for reports.
     * Typically you will use this method by doing:
     * QString myStyle = QgsApplication::reportStyleSheet();
     * textBrowserReport->document()->setDefaultStyleSheet(myStyle);
     * @return QString containing the CSS 2.1 compliant stylesheet.
     * @note you can use the special Qt extensions too, for example
     * the gradient fills for backgrounds.
     */
    static QString reportStyleSheet();

    /** Convenience function to get a summary of the paths used in this
     * application instance useful for debugging mainly.*/
    static QString showSettings();

    /** Register OGR drivers ensuring this only happens once.
     * This is a workaround for an issue with older gdal versions that
     * caused duplicate driver name entries to appear in the list
     * of registered drivers when QgsApplication::registerOgrDrivers was called multiple
     * times.
     */
    static void registerOgrDrivers();

    /**Converts absolute path to path relative to target */
    static QString absolutePathToRelativePath( QString apath, QString targetPath );
    /**Converts path relative to target to an absolute path */
    static QString relativePathToAbsolutePath( QString rpath, QString targetPath );

    /** Indicates whether running from build directory (not installed) */
    static bool isRunningFromBuildDir() { return ABISYM( mRunningFromBuildDir ); }
#ifdef _MSC_VER
    static QString cfgIntDir() { return ABISYM( mCfgIntDir ); }
#endif
    /** Returns path to the source directory. Valid only when running from build directory */
    static QString buildSourcePath() { return ABISYM( mBuildSourcePath ); }
    /** Returns path to the build output directory. Valid only when running from build directory */
    static QString buildOutputPath() { return ABISYM( mBuildOutputPath ); }

    /** Sets the GDAL_SKIP environment variable to include the specified driver
     * and then calls GDALDriverManager::AutoSkipDrivers() to unregister it. The
     * driver name should be the short format of the Gdal driver name e.g. GTIFF.
     */
    static void skipGdalDriver( QString theDriver );

    /** Sets the GDAL_SKIP environment variable to exclude the specified driver
     * and then calls GDALDriverManager::AutoSkipDrivers() to unregister it. The
     * driver name should be the short format of the Gdal driver name e.g. GTIFF.
     */
    static void restoreGdalDriver( QString theDriver );

    /** Returns the list of gdal drivers that should be skipped (based on
     * GDAL_SKIP environment variable)
     */
    static QStringList skippedGdalDrivers() { return ABISYM( mGdalSkipList ); }

    /** Apply the skipped drivers list to gdal
     * @see skipGdalDriver
     * @see restoreGdalDriver
     * @see skippedGdalDrivers */
    static void applyGdalSkippedDrivers();

    /** Get maximum concurrent thread count
     * @note added in 2.4 */
    static int maxThreads() { return ABISYM( mMaxThreads ); }
    /** Set maximum concurrent thread count
     * @note must be between 1 and \#cores, -1 means use all available cores
     * @note added in 2.4 */
    static void setMaxThreads( int maxThreads );

#ifdef ANDROID
    //dummy method to workaround sip generation issue issue
    bool x11EventFilter( XEvent * event )
    {
      return 0;
    }
#endif

  signals:
    //! @note not available in python bindings
    void preNotify( QObject * receiver, QEvent * event, bool * done );

  private:
    static QObject* ABISYM( mFileOpenEventReceiver );
    static QStringList ABISYM( mFileOpenEventList );

    static QString ABISYM( mPrefixPath );
    static QString ABISYM( mPluginPath );
    static QString ABISYM( mPkgDataPath );
    static QString ABISYM( mLibraryPath );
    static QString ABISYM( mLibexecPath );
    static QString ABISYM( mThemeName );
    static QStringList ABISYM( mDefaultSvgPaths );
    static QMap<QString, QString> ABISYM( mSystemEnvVars );

    static QString ABISYM( mConfigPath );

    /** true when running from build directory, i.e. without 'make install' */
    static bool ABISYM( mRunningFromBuildDir );
    /** path to the source directory. valid only when running from build directory. */
    static QString ABISYM( mBuildSourcePath );
#ifdef _MSC_VER
    /** configuration internal dir */
    static QString ABISYM( mCfgIntDir );
#endif
    /** path to the output directory of the build. valid only when running from build directory */
    static QString ABISYM( mBuildOutputPath );
    /** List of gdal drivers to be skipped. Uses GDAL_SKIP to exclude them.
     * @see skipGdalDriver, restoreGdalDriver */
    static QStringList ABISYM( mGdalSkipList );
    /**
     * @note added in 2.4 */
    static int ABISYM( mMaxThreads );
};

#endif