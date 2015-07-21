<?php


class ContentTextMigration extends Migration 
{

    public function __construct() 
    {
        parent::__construct();
        
        $this->description = t('Import of Text Content.');
        
        $fields = array (
            'id' => 'Unique page ID',
            'title' => 'Page title',
        );
        
        $contentDir = realpath(DRUPAL_ROOT . '/../content/pages');
        
        $contentList = new EllieSectionMigrateList($contentDir, 'text');
        $contentItem = new EllieSectionMigrateItem($contentDir, 'text');
        
        $this->source = new MigrateSourceList($contentList, $contentItem, $fields );
            
        $this->destination = new MigrateDestinationEntityAPI('content_elements', 'text_block');
        
        //Source and destination relation for rollbacks
        $this->map = new MigrateSQLMap(
          $this->machineName,
          array(
            'id' => array(
              'type' => 'varchar',
              'length' => 255,
              'not null' => TRUE,
            )
          ),
          MigrateDestinationEntityAPI::getKeySchema('content_elements', 'text_block')
        );
        
        //Unmigrate
        $unmigrate = new buildUnMigrateList();
        
        $unmigrateData = $unmigrate->getListItems();
        
        $this->addUnmigratedDestinations($unmigrateData);

    }   
    
}
