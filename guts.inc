<?php

/**
 * @file
 * Migrate Demo for basic page and article
 */

/*
 * 
 * This post describes how to handle redirects:
 *    http://www.group42.ca/creating_url_redirects_with_migrate_module
 *
 * Comments suggest that it is now as easy as:
 *
 *    $this->addFieldMapping('migrate_redirects', 'old_legacy_path');
 *
 * But I preseve the whole post because it may be simpler for us to process
 * TeamSite DCRs (and symbolic links) separately at the end.
 */

/**
 * Always use a base migration
 */
abstract class DemoBaseMigration extends XMLMigration {

	public function __construct() {
		// Always call the parent constructor first for basic setup
		parent::__construct();

		// With migrate_ui enabled, migration pages will indicate people involved in
		// the particular migration, with their role and contact info. We default the
		// list in the shared class; it can be overridden for specific migrations.
		$this->team = array(
				new MigrateTeamMember('Dan Davis', 'dan@danizen.net', t('Systems/Applications Architect')),
				);

		// Our source directory for XML files will default to our own directory
		$this->xml_dir =  DRUPAL_ROOT.'/'.drupal_get_path('module', 'migdemo').'/xml';

		// our primary ID is a string and represents the TeamSite DCR path
		$this->map = new MigrateSQLMap($this->machineName,
				array(
					'source' => array(
						'type' => 'varchar',
						'length' => '255',
						'not null' => TRUE,
						'description' => 'TeamSite DCR Path'
						)
					),
				MigrateDestinationNode::getKeySchema()
				);
	}

	/**
	 * build the source from the XML file path and XPath
	 */
	public function build_source (&$fields_array, $xmlfile_name, $items_xpath, $id_xpath = 'source') {
		$xml_dir =  DRUPAL_ROOT.'/'.drupal_get_path('module', 'migdemo').'/xml';
		$items_url = $xml_dir . '/'. $xmlfile_name;
		$items_class = new MigrateItemsXML($items_url, $items_xpath, $id_xpath);
		$this->source = new MigrateSourceMultiItems($items_class, $fields_array);
	}

	/**
	 * Build the destination from the node type machine name
	 */
	public function build_dest ($node_type) {
		$this->node_type = $node_type;
		$this->destination = new MigrateDestinationNode($node_type);
	}

	/**
	 * Strip a leading '/' from the path or path alias
	 */
	public function strip_leading_slash ($value) {
		if ($value && $value{0} === '/')
			$value = substr($value, 1);
		return $value;
	}

	/**
	 * Remove tags is meant only for simple HTML markup
	 */
	public function remove_tags ($value) {
		return preg_replace('/<\/?[^>]+>/', '', $value);
	}

	/**
	 * Return basename of a page /htdocs/hmd/about/exhibition/img/blah.jpg
	 */
	public function basename ($value) {
		return basename($value);
	}

	/**
	 *  disable path auto in base class
	 */
	public function prepare (&$row) {
		if ($this->disable_pathauto) {
			$row->path['pathauto'] = 0;
		}
	}
}

/**
 */
class DemoPageMigration extends DemoBaseMigration {
  public function __construct() {

    // always call parent
    parent::__construct();

    $this->description = t('Import basic pages from xml/basics.xml');

    // setup our source - factsheets.xml on disk
    $fields = array(
        'source' => t('Filesystem path of existing file?  Database ID?'),
			  'alias' => t('Relative URI of existing content'),
        'title' => t('Title for the new node'),
        'body' => t('Main content for the new node'),
        'created' => t('Date of first publication'),
        'changed' => t('Date of last modification'),
    );

    // Set up our source
    $this->build_source($fields, 'basics.xml', '/pages/page');

    // Set up our destination - nodes of type nlm_migrate_factsheet
    $this->build_dest('page');

    // field mappings
    
  }

}

/**
 */
class DemoArticleMigration extends DemoBaseMigration {
	public function __construct() {

		// always call parent
		parent::__construct();

		$this->description = t('Import News Items from xml/news.xml');

		// setup our source - factsheets.xml on disk
		$fields = array(
				'source' => t('Path of DCR file witihn TeamSite'),
				'title' => t('Title for the new node - derived by processing Heading and Title items'),
				'body' => t('Main content for the factsheet'),
				'created' => t('Date of first publication'),
				'changed' => t('Date of last modification'),
				);
		// Set up our source
		$this->build_source($fields, 'news.xml', '/newslist/newsitem');

		// Set up our destination - nodes of type nlm_migrate_factsheet
		$this->build_dest('article');

    // field mappings
	}
}

