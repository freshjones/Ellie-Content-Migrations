<?php 
function ellie_content_migrations_migrate_api() 
{

  $api = array(
    'api' => 2,
	'groups' => array(
		'pages' => array(
			'title' => t('Pages'),
		),
    ),
	'migrations' => array(
			
		'pages' => array(
				'class_name' => 'PagesMigration',
				'group_name' => 'pages',
		),
	     
    ),
          
  );

  return $api;

}