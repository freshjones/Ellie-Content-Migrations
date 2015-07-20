<?php

class EllieContentMigrateList extends MigrateList 
{

    private $path;
    private $type;
    
    public function __construct($list_url, $type = 'pages') 
    {
        parent::__construct();
        $this->path = $list_url;
        $this->type = $type;
    }
    
    /**
     * Our public face is the dir we're getting items from
     *
     * @return string
     */
    public function __toString() 
    {
        return $this->path;
    }
    
    /**
     * Load each directory within the content type, and return an array of the IDs for each directory.
     *
     * @return array
     */
    public function getIdList() 
    {
        
        $idList = array();
        
        foreach (new DirectoryIterator($this->path) as $fileInfo) 
        {
            if($fileInfo->isDot()) continue;
            if( $fileInfo->isDir() )
            {
                $idList[] = $fileInfo->getFilename();
            }
        }
        
        if(!empty($idList))
        {
            return $idList;
        }
        
        Migration::displayMessage(t('Loading of !listurl failed:',
            array('!listurl' => $this->listUrl)));
        
        return null;

    }

    public function computeCount() 
    {
        $count = 0;
        
        $idList = array();
        
        foreach (new DirectoryIterator($this->path) as $fileInfo)
        {
            if($fileInfo->isDot()) continue;
            if( $fileInfo->isDir() )
            {
                $idList[] = $fileInfo->getFilename();
            }
        }
        
        if(!empty($idList))
        {
            return count($idList);
        }
        
        return $count;
        
    }
    
}

class EllieContentMigrateItem extends MigrateItem
{

    protected $path;
    protected $type;
    protected $itemID = NULL;
    protected $item = NULL;
    
    public function __construct($path, $type) 
    {
        parent::__construct();
        $this->path = $path;
        $this->type = $type;
    }
    
    public function getItem($id) 
    {
        
        $this->itemID = $id;
        
        $itemPath = $this->path . '/' . $id;
        
        if(is_dir($itemPath))
        {
          
            $filename = $itemPath . '/content.txt';

            if(file_exists($filename))
            {
                $itemContent = file_get_contents($filename);
                
                if(strlen($itemContent))
                {
                    $this->setItemObject($itemContent);
                    
                }
                
            }
         
        } 
        
        return $this->getItemObject();
        
    }
    
    private function setItemObject($item)
    {
        
        $elementsArray = preg_split ('/\[(.*)\]/', $item, -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
        
        $counter = 0;
        if(!empty($elementsArray))
        {
            $objArray = array();
            foreach($elementsArray AS $key => $value)
            {
                if( ($counter++ % 2) == 0 ) 
                {
                    $objArray[$value] = $elementsArray[$key + 1];
                }
            }
            
            $this->item = isset($objArray) && !empty($objArray) ? (object)$objArray : NULL;
            
        }

    }
    
    private function getItemObject()
    {
        
        if(is_null($this->item) )
        {
            $migration = Migration::currentMigration();
            $message =  t('Loading of !objecturl failed:', array('!objecturl' => $this->path));
            $migration->getMap()->saveMessage(
                array($this->itemID), $message, MigrationBase::MESSAGE_ERROR);
            return NULL;
        }
        
        return $this->item;
        
    }

}

class PagesMigration extends Migration 
{

  public function __construct($arguments)
  {
     
    parent::__construct($arguments);

    $this->description = t('Page Migrations');

    $this->map = new MigrateSQLMap
    (
      $this->machineName,
      array(
        'id' => array
                (
                    'type' => 'varchar',
                    'length' => 255,
                    'not null' => TRUE,
                    'description' => 'Page ID',
                ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $fields = array (
      'id' => 'Unique page ID',
      'title' => 'Page title',
      'body' => 'Page body',
    );
    
    $contentDir = realpath(DRUPAL_ROOT . '/../content/pages');
    
    $this->source = new MigrateSourceList(new EllieContentMigrateList($contentDir, 'pages'), new EllieContentMigrateItem($contentDir, 'pages'), $fields );

    $this->destination = new MigrateDestinationNode('pages');
      
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);

    /* DONT MIGRATE */
        
    //Unmigrate        
    $unmigrate = new buildUnMigrateList();

    $unmigrate->setListItems(array('body:format','body:summary'));
    $unmigrate->setListItems(array('path'));

    $unmigrate->setListItems(array('metatag_title','metatag_description','metatag_abstract', 'metatag_keywords'));
    $unmigrate->setListItems(array('metatag_robots','metatag_news_keywords','metatag_standout', 'metatag_rating'));
    $unmigrate->setListItems(array('metatag_referrer','metatag_generator','metatag_rights', 'metatag_image_src'));
    $unmigrate->setListItems(array('metatag_canonical','metatag_shortlink','metatag_publisher', 'metatag_author'));
    $unmigrate->setListItems(array('metatag_original-source','metatag_prev','metatag_next', 'metatag_revisit-after'));
    $unmigrate->setListItems(array('metatag_content-language','metatag_geo.position','metatag_geo.placename', ));
    $unmigrate->setListItems(array('metatag_geo.region', 'metatag_icbm', 'metatag_refresh'));

    $unmigrateData = $unmigrate->getListItems();
        
    $this->addUnmigratedDestinations($unmigrateData);

  }
  
  public function complete($entity, stdClass $row) 
  {
      
      $item = array(
          'link_path' => 'node/' . $entity->nid,
          'link_title' => $entity->title,
          'menu_name' => 'main-menu',
          'weight' => 0,
          'expanded' => 0,
      );
      
      $item_id = menu_link_save($item);
      
      if($row->id == 'home')
      {
          variable_set('site_frontpage', 'node/' . $entity->nid);
      }

  }
  
}