<?php 
function ellie_content_migrations_migrate_api() 
{

  $api = array(
    'api' => 2,
	'groups' => array
    (
		'pages' => array
	    (
			'title' => t('Pages'),
		),
	    'content' => array(
	        'title' => t('Content'),
	    ),
    ),
	'migrations' => array
    (
			
		'Pages' => array(
				'class_name' => 'PagesMigration',
				'group_name' => 'pages',
		),
	    'ContentSections' => array(
    	        'class_name' => 'ContentSectionMigration',
    	        'group_name' => 'content',
	    ),
	    'ContentText' => array(
    	        'class_name' => 'ContentTextMigration',
    	        'group_name' => 'content',
	    ),
	    
    ),
          
  );

  return $api;

}