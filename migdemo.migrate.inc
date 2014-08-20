<?php

/**
 * @file
 * Because the name of this file is the module name plus '.migrate.inc', when
 * hook_migrate_api is invoked by the Migrate module this file is automatically
 * loaded - thus, you don't need to implement your hook in the .module file.
 */

/*
 * Hook migrate API to register our migration
 */
function migdemo_migrate_api() {
  $api = array(
      'api' => 2,
      'migrations' => array(
			  // Step 1
        'DemoPage' => array('class_name' => 'DemoPageMigration'),
        'DemoArticle' => array('class_name' => 'DemoArticleMigration'),
        ),
      );
  return $api;
}
