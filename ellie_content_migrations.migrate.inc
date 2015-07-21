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
	    'sections' => array(
	        'title' => t('Sections'),
	    ),
	    'content' => array(
	        'title' => t('Content'),
	    ),
    ),
	'migrations' => array
    (
			
		'pages' => array(
				'class_name' => 'PagesMigration',
				'group_name' => 'pages',
		),
	    'sections' => array(
    	        'class_name' => 'SectionMigration',
    	        'group_name' => 'sections',
	    ),
	    'content_text' => array(
    	        'class_name' => 'ContentTextMigration',
    	        'group_name' => 'content',
	    ),
	    
    ),
          
  );

  return $api;

}